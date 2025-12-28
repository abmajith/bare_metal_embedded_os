# Setting up NFS File System on the Host

```bash
sudo apt install nfs-kernel-server
sudo cat /etc/exports
/srv/nfs/<sbc>-root <subnet-address>/24(rw,sync,no_subtree_check,no_root_squash)
# for example
/srv/nfs/rpi3b-root 192.168.1.0/24(rw,sync,no_subtree_check,no_root_squash)

sudo exports -arv
sudo ufw status
sudo ufw allow from 192.168.1.0/24 to any port nfs
sudo ufw reload 
```

So you can keep the busybox or custom build root file system for the SBC in the folder `/srv/nfs/<sbc>-root`


# To share the Updated Kernel and Files at Boot Stage From Host
To share the updated linux image or dtb file, overlays to boot the board to load the kernel and use the nfs,
we could use tftp

```bash
mkdir -p /srv/tftp/

cat  /etc/default/tftpd-hpa 
# /etc/default/tftpd-hpa
TFTP_USERNAME="tftp"
TFTP_DIRECTORY="/srv/tftp"
TFTP_ADDRESS="[::]:69"
TFTP_OPTIONS="--secure"

sudo exports -arv
sudo service tftpd-hpa restart
sudo service tftpd-hpa status
```

