
# Copyright (C) 2018-2020 Ruilin Peng (Nick) <pymumu@gmail.com>.
#
# smartdns is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# smartdns is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


BIN=smartdns 
ifeq ($(MERLINR_NAME), $(filter $(MERLINR_NAME), K3 SBRAC1900P SBRAC3200P RTAC68U))
OBJS_LIB=lib/rbtree.o lib/art.o lib/bitops.o lib/radix.o lib/conf.o lib/ifaddrs.o
else
OBJS_LIB=lib/rbtree.o lib/art.o lib/bitops.o lib/radix.o lib/conf.o
endif
OBJS=smartdns.o fast_ping.o dns_client.o dns_server.o dns.o util.o tlog.o dns_conf.o dns_cache.o http_parse.o $(OBJS_LIB)
ifeq ($(MERLINR_NAME), $(filter $(MERLINR_NAME), GTAC5300 GTAC2900 RTAC86U R7900P R8000P))
CFLAGS +=-O1 -g -Wall -Wstrict-prototypes -fno-omit-frame-pointer -Wstrict-aliasing 
else
CFLAGS +=-O2 -g -Wall -Wstrict-prototypes -fno-omit-frame-pointer -Wstrict-aliasing 
endif
CFLAGS +=-I./include -I$(TOP)/openssl/include

CFLAGS += -DBASE_FILE_NAME=\"$(notdir $<)\"
LDFLAGS +=-L$(TOP)/openssl -lssl -lcrypto
CFLAGS += -DSMARTDNS_VERION=\"$(MERLINR_VER_MINOR)\"
CXXFLAGS=-O2 -g -Wall -std=c++11 
CXXFLAGS +=-Iinclude
#ifeq ($(STATIC), yes)
#LDFLAGS += -lssl -lcrypto -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -ldl -static
#else
LDFLAGS += -lssl -lcrypto -lpthread 
#endif

.PHONY: all

all: $(BIN)
 
$(BIN) : $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(BIN)
