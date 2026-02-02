#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <getopt.h>

// 1 ms
#define TARGET_LATENCY_NS 1000000
#define SAMPLES 5000
#define MEM_SIZE (16 * 1024 * 1024)

// Page Fault Stress: Force kernel to map/unmap pages
void *page_fault_worker(void *arg) 
{
    while(1) {
	char *ptr = mmap(NULL, MEM_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (ptr != MAP_FAILED)
	{
	   for (size_t i = 0; i < MEM_SIZE; i += 4096) ptr[i] = 1;
	   munmap(ptr, MEM_SIZE);
	} 
	else 
	{
	   printf("Failed to get the mmap ptr, returning earlier from page fault worker\n");
	   return NULL;
	}
    }
    return NULL;
}

// Thread function to hammer the CPU
void *cpu_worker(void *arg) 
{
    unsigned long i = 0;
    while(1) { i++; } // Simple infinite increment
    return NULL;
}

void *mem_worker(void *arg)
{
   char *buf = malloc(MEM_SIZE);
    while(1) {
        for (size_t i = 0; i < MEM_SIZE; i += 64) buf[i] ^= 1; // Stride for cache misses
    }
   return NULL;
}

void *nfs_worker(void *arg) {
    char filename[64];
    sprintf(filename, "/home/nfs_stress_%ld.tmp", (long)pthread_self());
    char *data = malloc(64 * 1024); // 64KB buffer
    memset(data, 0x55, 64 * 1024);

    FILE *fp = fopen(filename, "w+");
    if (!fp) return NULL;

    while(1) {
        rewind(fp);
        fwrite(data, 1, 64 * 1024, fp);
        fflush(fp);
        fsync(fileno(fp)); // Crucial: This forces the NFS "round-trip"
    }

    // Cleanup if thread is canceled
    fclose(fp);
    unlink(filename);
    free(data);
    return NULL;
}


int main(int argc, char* argv[]) 
{
    struct timespec next, end, now;
    long max_jitter = 0;
    long total_latency = 0;
    int num_stress_threads = 0;
	
    int opt, do_cpu = 0, do_page = 0, do_mem = 0, do_nfs = 0, use_lock = 0;
    pthread_t threads[10];
    int thread_count = 0;

    while ((opt = getopt(argc, argv, "cpmsn")) != -1) 
    {
        switch (opt) 
        {
            case 'c': do_cpu = 1; break;
            case 'p': do_page = 1; break;
            case 's': do_mem = 1; break;
            case 'm': use_lock = 1; break;
            case 'n': do_nfs =1; break;
        }
    }
    if (use_lock && mlockall(MCL_CURRENT | MCL_FUTURE) == -1) perror("mlockall");
    if (do_cpu) pthread_create(&threads[thread_count++], NULL, cpu_worker, NULL);
    if (do_page) pthread_create(&threads[thread_count++], NULL, page_fault_worker, NULL);
    if (do_mem) pthread_create(&threads[thread_count++], NULL, mem_worker, NULL);
    if (do_nfs) pthread_create(&threads[thread_count++], NULL, nfs_worker, NULL);
    printf("Running benchmark... (Lock: %s)\n", use_lock ? "YES" : "NO");

    clock_gettime(CLOCK_MONOTONIC, &next);
    for (int i = 0; i < SAMPLES; i++) 
    {
        next.tv_nsec += TARGET_LATENCY_NS;
        if (next.tv_nsec >= 1000000000) {
            next.tv_nsec -= 1000000000;
            next.tv_sec++;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);        
        clock_gettime(CLOCK_MONOTONIC, &end);

        long jitter = (end.tv_sec - next.tv_sec) * 1000000000L + (end.tv_nsec - next.tv_nsec);
	    total_latency += (jitter < 0) ? 0 : jitter;        
        if (jitter > max_jitter) max_jitter = jitter;
    }
    
    // Clean up
    for (int i = 0; i < thread_count; i++) pthread_cancel(threads[i]);
    if (use_lock) munlockall();

    printf("Stressed Max Jitter: %ld us\n", max_jitter / 1000);
    printf("Avg Jitter: %ld us\n", (total_latency / SAMPLES) / 1000 );
    return 0;
}
