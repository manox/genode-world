OPENCV_PORT_DIR := $(call select_from_ports,opencv)
OPENCV_DIR      := $(OPENCV_PORT_DIR)/src/lib/opencv-3.1.0

SRC_CC += $(notdir $(wildcard $(OPENCV_DIR)/modules/core/src/*.cpp))

vpath %.cpp $(OPENCV_DIR)/modules/core/src

INC_DIR += $(OPENCV_PORT_DIR)/include

CC_OPT += -D__OPENCV_BUILD=1

LIBS += libc libc_pipe stdcxx zlib libm libpng jpeg

SHARED_LIB = yes

# create opencv2/opencv_modules.hpp
opencv2/opencv_modules.hpp:
	$(VERBOSE)mkdir opencv2
	$(VERBOSE)echo "#define HAVE_OPENCV_CORE" > opencv2/opencv_modules.hpp
	$(VERBOSE)echo "#define HAVE_OPENCV_IMGCODECS" >> opencv2/opencv_modules.hpp
	$(VERBOSE)echo "#define HAVE_OPENCV_IMGPROC" >> opencv2/opencv_modules.hpp
	$(VERBOSE)echo "#define HAVE_OPENCV_FLANN" >> opencv2/opencv_modules.hpp
	$(VERBOSE)echo "#define HAVE_OPENCV_FEATURES2D" >> opencv2/opencv_modules.hpp
	$(VERBOSE)echo "#define HAVE_OPENCV_CALIB3D" >> opencv2/opencv_modules.hpp
	$(VERBOSE)echo "#define HAVE_OPENCV_ML" >> opencv2/opencv_modules.hpp

$(SRC_CC:.cpp=.o): opencv2/opencv_modules.hpp

# create version_string.inc
version_string.inc:
	$(VERBOSE)echo "\"Genode port of OpenCV 3.1.0\"" > $@

$(SRC_CC:.cpp=.o): version_string.inc

# create cvconfig.h
cvconfig.h:
	$(VERBOSE)echo "#define BUILD_SHARED_LIBS" > $@
	$(VERBOSE)echo "#define CUDA_ARCH_BIN \"\"" >> $@
	$(VERBOSE)echo "#define CUDA_ARCH_FEATURES \"\"" >> $@
	$(VERBOSE)echo "#define CUDA_ARCH_PTX \"\"" >> $@
	$(VERBOSE)echo "#define HAVE_PTHREADS" >> $@

$(SRC_CC:.cpp=.o): cvconfig.h

# create custom_hal.hpp
custom_hal.hpp:
	$(VERBOSE)echo "#ifndef _CUSTOM_HAL_INCLUDED_" > $@
	$(VERBOSE)echo "#define _CUSTOM_HAL_INCLUDED_" >> $@
	$(VERBOSE)echo "#endif" >> $@

$(SRC_CC:.cpp=.o): custom_hal.hpp

# create opencl_kernels_core.hpp
opencl_kernels_core.hpp:
	$(VERBOSE)echo "#include \"opencv2/core/ocl.hpp\"" > $@
	$(VERBOSE)echo "#include \"opencv2/core/ocl_genbase.hpp\"" >> $@
	$(VERBOSE)echo "#include \"opencv2/core/opencl/ocl_defs.hpp\"" >> $@

$(SRC_CC:.cpp=.o): opencl_kernels_core.hpp
