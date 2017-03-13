# \brief  PL IRQ specific for zynq systems
# \author Mark Albers
# \date   2016-01-25

TARGET   = zynq_pl_irq_handler

SRC_CC   = main.cc
LIBS     = base stdcxx
INC_DIR += $(PRG_DIR)

vpath main.cc $(PRG_DIR)

