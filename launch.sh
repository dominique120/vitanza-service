#!/bin/bash
export vts_ip=$(hostname -I | awk '{print $1}')
./vts "$vts_ip" "80"