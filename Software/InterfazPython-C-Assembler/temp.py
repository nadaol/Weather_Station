from ctypes import *
import sys

libPyC = CDLL("./libpyc.so")

#call C function to check connection
libPyC.connect()

#calling addNum() C function
#it returns addition of two numbers
tempC = 20.5
print "Pyhton -- Temp: ", tempC, " C"
#libCalc.fahrenheit.argtypes = c_float
#libCalc.fahrenheit.restype = c_int
tempF = libPyC.fahrenheit(c_float(tempC))
print "Pyhton -- Temp: ", tempF, " F"