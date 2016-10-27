OPENCV_PORT_DIR := $(call select_from_ports,opencv)
OPENCV_DIR      := $(OPENCV_PORT_DIR)/src/lib/opencv-3.1.0

SRC_CC += $(notdir $(wildcard $(OPENCV_DIR)/modules/features2d/src/*.cpp))
SRC_CC += $(notdir $(wildcard $(OPENCV_DIR)/modules/features2d/src/kaze/*.cpp))

vpath %.cpp $(OPENCV_DIR)/modules/features2d/src
vpath %.cpp $(OPENCV_DIR)/modules/features2d/src/kaze

INC_DIR += $(OPENCV_PORT_DIR)/include

CC_OPT += -D__OPENCV_BUILD=1

LIBS += libc libc_pipe stdcxx zlib libm libpng jpeg opencv_core

SHARED_LIB = yes

# create cvconfig.h
cvconfig.h:
	$(VERBOSE)echo "#define BUILD_SHARED_LIBS" > $@
	$(VERBOSE)echo "#define CUDA_ARCH_BIN \"\"" >> $@
	$(VERBOSE)echo "#define CUDA_ARCH_FEATURES \"\"" >> $@
	$(VERBOSE)echo "#define CUDA_ARCH_PTX \"\"" >> $@
	$(VERBOSE)echo "#define HAVE_PTHREADS" >> $@

$(SRC_CC:.cpp=.o): cvconfig.h

# create opencl_kernels_features2d.hpp
opencl_kernels_features2d.hpp:
	$(VERBOSE)echo "#include \"opencv2/core/ocl.hpp\"" > $@
	$(VERBOSE)echo "#include \"opencv2/core/ocl_genbase.hpp\"" >> $@
	$(VERBOSE)echo "#include \"opencv2/core/opencl/ocl_defs.hpp\"" >> $@

$(SRC_CC:.cpp=.o): opencl_kernels_features2d.hpp
