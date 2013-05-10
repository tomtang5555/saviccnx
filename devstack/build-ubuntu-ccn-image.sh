#!/bin/sh

IMG=$1
CACHE_DIR=$2

UBUNTU_PASS=savi
PASS_FILE=$CACHE_DIR/passfile

touch $PASS_FILE
echo $UBUNTU_PASS > $PASS_FILE
echo $UBUNTU_PASS >> $PASS_FILE
echo "\n" >> $PASS_FILE

SRC_URL=http://cloud-images.ubuntu.com/precise/current/precise-server-cloudimg-amd64-root.tar.gz
SRC_CACHE=$CACHE_DIR/precise-server-cloudimg-amd64-root.tar.gz

#SRC_URL=http://cloud-images.ubuntu.com/precise/current/precise-server-cloudimg-i386-root.tar.gz
#SRC_CACHE=$CACHE_DIR/precise-server-cloudimg-i386-root.tar.gz

CCN_SRC_URL=http://www.ccnx.org/releases/ccnx-0.7.1.tar.gz
CCN_TAR=/home/ubuntu/ccnx-0.7.1.tar.gz
CCN_FOLDER=/home/ubuntu/
KERNEL_VER=39

if [ -f "$IMG" ]; then
exit 0
fi

if ! [ -f "$SRC_CACHE" ]; then
wget "$SRC_URL" -O - > "$SRC_CACHE"
fi

#image size in MB
IMAGE_SIZE=2000
dd if=/dev/zero of="$IMG" bs=1M count=0 seek=$IMAGE_SIZE
mkfs -F -t ext4 "$IMG"
MNT_DIR=`mktemp -d`
sudo mount -o loop "$IMG" "${MNT_DIR}"
sudo tar -C "${MNT_DIR}" -xzf "$SRC_CACHE"
sudo mv "${MNT_DIR}/etc/resolv.conf" "${MNT_DIR}/etc/resolv.conf_orig"
sudo cp /etc/resolv.conf "${MNT_DIR}/etc/resolv.conf"
sudo chroot "${MNT_DIR}" apt-get -y install "linux-image-3.2.0-${KERNEL_VER}-generic" vlan open-iscsi
sudo chroot "${MNT_DIR}" passwd ubuntu < $PASS_FILE

# CCN addition:
#  Get required packages
sudo chroot "${MNT_DIR}" mount -t proc proc /proc
#sudo chroot "${MNT_DIR}" export LTSP_HANDLE_DAEMONS=false
sudo chroot "${MNT_DIR}" apt-get update
sudo chroot "${MNT_DIR}" apt-get -y upgrade
sudo chroot "${MNT_DIR}" apt-get -y install vim git-core openjdk-6-jdk gcc libssl-dev libexpat1-dev libpcap-dev libxml2-utils ant athena-jot python-dev build-essential iperf

#  Get CCNx
sudo chroot "${MNT_DIR}" wget "$CCN_SRC_URL" -O - > "$CCN_TAR"
sudo chroot "${MNT_DIR}" tar -zxf /home/ubuntu/ccnx-0.7.1.tar.gz -C "$CCN_FOLDER"
#  Need to copy other files:
#    ONL CCNx Utilities
#    ssh related files
#
#  Change home folder owner
sudo chroot "${MNT_DIR}" chown -R ubuntu:ubuntu /home/ubuntu/
#  Clean up
sudo chroot "${MNT_DIR}" umount /proc

# For CMU guys
#sudo adduser cmu
# answer some questions from adduser
#sudo adduser cmu admin
#groups cmu
# Edit /etc/ssh/sshd_config file to allow plain text
# Or install CMU's pubkey


sudo mv "${MNT_DIR}/etc/resolv.conf_orig" "${MNT_DIR}/etc/resolv.conf"
sudo cp "${MNT_DIR}/boot/vmlinuz-3.2.0-${KERNEL_VER}-generic" "$CACHE_DIR/kernel"
sudo chmod a+r "$CACHE_DIR/kernel"
sudo chown $USER:$USER "$CACHE_DIR/kernel"
cp "${MNT_DIR}/boot/initrd.img-3.2.0-${KERNEL_VER}-generic" "$CACHE_DIR/initrd"
sudo umount "${MNT_DIR}"

