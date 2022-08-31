#!/usr/bin/env sh
mkdir cosmo
curl https://justine.lol/cosmopolitan/ape.o > cosmo/ape.o
curl https://justine.lol/cosmopolitan/ape.lds > cosmo/ape.lds
curl https://justine.lol/cosmopolitan/cosmopolitan.a > cosmo/cosmopolitan.a
curl https://justine.lol/cosmopolitan/cosmopolitan.h > cosmo/cosmopolitan.h
curl https://justine.lol/cosmopolitan/crt.o > cosmo/crt.o

curl https://justine.lol/cosmopolitan/cosmopolitan.tar.gz > cosmo.tar.gz && tar xvf cosmo.tar.gz && cp -r cosmopolitan/libc cosmo/libc

rm -rf cosmo.tar.gz
rm -rf cosmopolitan