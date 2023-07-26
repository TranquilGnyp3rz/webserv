#!/bin/bash

while :
do
    leaks webserv | grep bytes
    sleep 1
done