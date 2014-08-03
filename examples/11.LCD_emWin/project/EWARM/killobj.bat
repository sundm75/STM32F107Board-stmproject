@echo off

rd .\STM32F107Board /s /q
rd .\settings /s /q
del /Q *.dep
del /Q *.ewd
exit