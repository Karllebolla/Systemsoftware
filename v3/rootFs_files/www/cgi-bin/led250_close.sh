#!/bin/sh

PID=$(pidof led250)
kill $PID

PID=$(pidof led250)
kill $PID
