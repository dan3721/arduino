#!/bin/sh
(sleep 2; echo $@; sleep 2) | telnet 192.168.1.21 80
