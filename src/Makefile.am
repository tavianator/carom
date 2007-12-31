INCLUDES = -I$(top_srcdir)/src

LIBCAROM_VERSION = 0:0:0

CPP_SOURCES = mpfr_utils.cpp particle.cpp system.cpp utility.cpp utility/mesh.cpp utility/impenetrable.cpp utility/simple_body.cpp utility/rigid_body.cpp utility/forces.cpp
HPP_SOURCES = carom.hpp carom/mpfr_utils.hpp carom/scalar.hpp carom/vector.hpp carom/polymorphic_list.hpp carom/polymorphic_list/iterator.hpp carom/polymorphic_list/list.hpp carom/particle.hpp carom/body.hpp carom/system.hpp carom/utility.hpp carom/utility/mesh.hpp carom/utility/impenetrable.hpp carom/utility/simple_body.hpp carom/utility/rigid_body.hpp carom/utility/fluid_body.hpp carom/utility/forces.hpp

nobase_include_HEADERS = $(HPP_SOURCES)

lib_LTLIBRARIES      = libcarom.la
libcarom_la_SOURCES  = $(CPP_SOURCES) $(HPP_SOURCES)
libcarom_la_LIBADD   = -lgmp -lmpfr -lboost_thread-mt
libcarom_la_LDFLAGS  = -version-info $(LIBCAROM_VERSION)
libcarom_la_CXXFLAGS = -Wall -Wno-non-template-friend
