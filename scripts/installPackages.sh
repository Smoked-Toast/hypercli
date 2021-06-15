#!/bin/bash

pacman -S virt-manager libvirt qemu bridge-utils -y


if ! systemctl is-active --quiet libvirtd; then
	systemctl enable libvirtd
	systemctl start libvirtd
fi
exit 0
