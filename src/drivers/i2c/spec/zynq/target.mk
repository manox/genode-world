# \brief  I2C specific for zynq systems
# \author Mark Albers
# \date   2015-03-12

TARGET   = zynq_i2c_drv
REQUIRES += zynq

SRC_CC   = main.cc
LIBS     = base
INC_DIR += $(PRG_DIR)

vpath main.cc $(PRG_DIR)

