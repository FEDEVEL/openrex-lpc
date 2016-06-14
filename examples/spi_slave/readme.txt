LPC application acts as SPI slave that send back upper-lowercase modified text,
previousely transmitted by master. For instance:

(master) hello WoRlD
(slave ) HELLO wOrLd

On master side (IMX6) you have to launch 'examples/spi_mcu_slave/spi_mcu_slave' 
app to comunicate with slave. 
