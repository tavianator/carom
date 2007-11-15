INCLUDES = -I$(top_srcdir)/src

LIBCAROM_VERSION = 0:0:0

CPP_SOURCES = particle.cpp system.cpp utility/mesh.cpp
HPP_SOURCES = carom.hpp carom/scalar.hpp carom/scalar/unary_proxy.hpp carom/scalar/binary_proxy.hpp carom/scalar/proxy.hpp carom/scalar/scalar.hpp carom/scalar/ops.hpp carom/vector.hpp carom/vector/proxy.hpp carom/vector/vector.hpp carom/vector/ops.hpp carom/polymorphic_list.hpp carom/polymorphic_list/iterator.hpp carom/polymorphic_list/list.hpp carom/particle.hpp carom/body.hpp carom/system.hpp carom/utility.hpp carom/utility/mesh.hpp carom/utility/impenetrable.hpp carom/utility/simple_body.hpp carom/utility/rigid_body.hpp carom/utility/fluid_body.hpp

nobase_include_HEADERS = $(HPP_SOURCES)

lib_LTLIBRARIES      = libcarom.la
libcarom_la_SOURCES  = $(CPP_SOURCES) $(HPP_SOURCES)
libcarom_la_LIBADD   = -lgmp -lmpfr
libcarom_la_LDFLAGS  = -version-info $(LIBCAROM_VERSION)
libcarom_la_CXXFLAGS = -Wall -Wno-non-template-friend
