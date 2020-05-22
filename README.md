# simple-container

> My goal is to run a common busybox Docker image, but without docker. Docker uses btrfs as a filesystem format for its images. Let’s try pulling the image and unpack it into a directory:


My implemetation of a container(chroot on steriods) using Linux kernel APIs, cgroups and runc

Containers a self-contained bundle of code and its dependencies, that can be shipped together and run in an isolated environment inside some host operating system.


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

### Running shell
To easier play around the new environment, let’s run a shell in the child process. Actually, let’s run arbitrary commands, much like docker run:

### Network interfaces

What our container launcher should do is to create a pair of peer interfaces, veth0 and veth1, link them to the br0 and set up routing within the container.

In the main() function, we need to add the following system calls
```
system("ip link add veth0 type veth peer name veth1");
system("ip link set veth0 up");
system("brctl addif br0 veth0");
```
After clone() is complete, we will add veth1 to the new child namespace:
```
char ip_link_set[4096];
snprintf(ip_link_set, sizeof(ip_link_set) - 1, "ip link set veth1 netns %d",
         pid);
system(ip_link_set);
```
Now if we run “ip link” in the container shell we will see a loopback interface, and some user@xxxx interface.
