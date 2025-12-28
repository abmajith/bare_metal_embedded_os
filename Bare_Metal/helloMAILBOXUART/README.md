## Hello UART0 MailBox Bare Metal RASPBERRY PI


Here, we have a simple *OS* kernel that prints a hello message on the host terminal via *RS232 USB* cable using mailbox and *UART0*.


Making them bootable will be the same procedure as
<a href="https://github.com/abmajith/bare_metal_embedded_os/tree/main/rpi_3bplus_BM/helloUART" class="custom-link">Hello UART1</a>.
Most of the code base is almost the same except for the mailbox, *uart_init* function code, and some *mmio* macros.


Note: For me, It works better compared to *hello UART1*, maybe a reduced
serial communication clock makes the host and target communicate better!
I set a *12 MHz* serial communication clock and communicate via *UART0*.


## *UART0* and *UART1* on the Raspberry Pi have different roles and clock configurations.
Typically, *UART0* is used as a more flexible general-purpose *UART*,
often with a lower clock speed that can be adjusted via the mailbox interface.
*UART1*, on the other hand, typically operates at a higher fixed clock speed, such as *48MHz*.


## Mailbox and UART0 Connection
The mailbox interface is used to send messages to the GPU to perform specific actions,
such as setting the clock rate for the *UART0* peripheral.
This configuration is necessary because the *UART0* clock rate needs to be
accurately set to ensure proper serial communication.
The mailbox request is typically made during the initialization process of *UART0*.


## Mailbox Setup:
The mailbox is a communication mechanism that involves writing messages to
specific memory-mapped registers. The structure of a mailbox message is
predefined and includes the message size, a request code,
and a series of tags that specify the operations to be performed.


## Mailbox Message Structure for UART0 Clock:
To set the *UART0* clock rate, a specific tag (0x00038002) is used.
This tag instructs the GPU to set the clock rate for the *UART0* peripheral.


## Mailbox Communication:
The ARM CPU writes the message to the mailbox and waits for a response from the GPU.
The GPU processes the message, sets the clock rate, and
then returns a response indicating the success or failure of the request.


##     UART0 Initialization:
After setting the clock rate, the *UART0* can be initialized 
with the desired baud rate and other configurations.

