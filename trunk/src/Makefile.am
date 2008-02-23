INCLUDES = -I$(top_srcdir)/src

LIBCAROM_VERSION = 0:0:0

CPP_SOURCES = mpfr_utils.cpp particle.cpp body.cpp system.cpp integrator.cpp mesh.cpp impenetrable.cpp simple_body.cpp rigid_body.cpp basic_forces.cpp electromagnetism.cpp
HPP_SOURCES = carom.hpp carom/mpfr_utils.hpp carom/scalar.hpp carom/vector.hpp carom/polymorphic_list.hpp carom/particle.hpp carom/body.hpp carom/system.hpp carom/integrator.hpp carom/mesh.hpp carom/impenetrable.hpp carom/simple_body.hpp carom/rigid_body.hpp carom/basic_forces.hpp carom/electromagnetism.hpp

nobase_include_HEADERS = $(HPP_SOURCES)

lib_LTLIBRARIES      = libcarom.la
libcarom_la_SOURCES  = $(CPP_SOURCES) $(HPP_SOURCES)
libcarom_la_LIBADD   = -lgmp -lmpfr -lboost_thread-mt
libcarom_la_LDFLAGS  = -version-info $(LIBCAROM_VERSION)
libcarom_la_CXXFLAGS = -Wall -Wno-non-template-friend
