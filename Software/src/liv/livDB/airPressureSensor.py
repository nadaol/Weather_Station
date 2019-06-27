#from Adafruit_BME280 import *

#sensor = BME280(t_mode=BME280_OSAMPLE_8, p_mode=BME280_OSAMPLE_8, h_mode=BME280_OSAMPLE_8)

#degrees = sensor.read_temperature()
#pascals = sensor.read_pressure()
#hectopascals = pascals / 100
#humidity = sensor.read_humidity()

#print 'Temp      = {0:0.3f} deg C'.format(degrees)
#print 'Pressure  = {0:0.2f} hPa'.format(hectopascals)
#print 'Humidity  = {0:0.2f} %'.format(humidity)

from Adafruit_BME280 import *

class  airPressureSensor:

  def  __init__(self):
    #set high resolution mode and I2C address
    self.sensor = BME280(t_mode=BME280_OSAMPLE_8, p_mode=BME280_OSAMPLE_8, h_mode=BME280_OSAMPLE_8)

  def  readAirPressure(self):
    temp = self.sensor.read_temperature()
    pressure = self.sensor.read_pressure()
    airPressure = pressure / 100.0
    return airPressure

  def  readTemperature(self):
    temperature = self.sensor.read_temperature()
    return temperature


  def  readAltitude(self):
    altitude = 0
    return 0

  #def  readHumidity(self):
  #  humidity = self.sensor.read_humidity()
  #  return humidity

#use default address 0x76, port 1 is default in Adafruit
if __name__ == "__main__":
    print 'testing BME280 air pressure Sensor'
    apSensor = airPressureSensor()
    t = apSensor.readTemperature()
    #a = apSensor.readAltitude()
    ap = apSensor.readAirPressure()
    #h = apSensor.readHumidity()
    
    print 'Temp = {0:0.1f} C'.format(t)
    print 'Air Pressure  = {0:0.2f} hPa'.format(ap)
    #print 'Humidity  = {0:0.2f} %'.format(h)
