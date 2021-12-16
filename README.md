# codepackage

Source Code Manager

# Installation

- Download latest release from https://github.com/JamesBremner/codepackage/releases/latest
- Unip to a writeable installation folder

You can create the installation folder anywhere you want.  I sugest the folder name ravencode.

# Usage

To list the package status

```
C:\Users\James\code\ravencode>pack

        Package         Status
           raven-set    !!! out of date!!!
              cutest    !!! out of date!!!
            runwatch    !!! out of date!!!
```

To update and publish the cutest package

```
C:\Users\James\code\ravencode>pack cutest
Cloning into 'raven-set'...
remote: Enumerating objects: 144, done.
remote: Counting objects: 100% (43/43), done.
remote: Compressing objects: 100% (33/33), done.
remote: Total 144 (delta 20), reused 29 (delta 10), pack-reused 101R
Receiving objects: 100% (144/144), 247.41 KiB | 1.17 MiB/s, done.
Resolving deltas: 100% (56/56), done.
cutest published to C:\Users\James\code\ravencode/pub/src
```

To build your application with the cutest code

```
g++ -o app.exe -c app.cpp -c ../ravencode/pub/src/cutest.cpp -I../ravencode/pub/src/
```
