# simple-container

My implemetaion of a container(chroot on steriods) using Linux kernel APIs, cgroups and runc

Containers a self-contained bundle of code and its dependencies, that can be shipped together and run in an isolated environment inside some host operating system.

My goal is to run a common busybox Docker image, but without docker. Docker uses btrfs as a filesystem format for its images. Let’s try pulling the image and unpack it into a directory:

```
mkdir rootfs
docker export $(docker create busybox) | tar -C rootfs -xvf -
```

Using ```clone(2)`` instead of ```fork()`` so that we can control what resources (filesystem, network) the forked process can see. Clone also allows us to pass the following flags;

Clone does pretty much the same, but allows you to pass flags, defining which resources you would want to share.

The following flags are allowed:

- CLONE_NEWNET – isolate network devices
- CLONE_NEWUTS – host and domain names (UNIX Timesharing System)
- CLONE_NEWIPC – IPC objects
- CLONE_NEWPID – PIDs
- CLONE_NEWNS – mount points (file systems)
- CLONE_NEWUSER – users and groups
