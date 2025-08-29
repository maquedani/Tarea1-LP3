# ====================== Configuración general ======================
CC       := gcc
CXX      := g++
CSTD     := -std=c11
CXXSTD   := -std=c++17
WARN     := -Wall -Wextra -Wpedantic
OPT      := -O2
INCS     := -Isrc
DEFS     :=

CFLAGS   := $(CSTD)   $(WARN) $(OPT) $(DEFS) $(INCS) -MMD -MP -D_POSIX_C_SOURCE=200809L
CXXFLAGS := $(CXXSTD) $(WARN) $(OPT) $(DEFS) $(INCS) -MMD -MP
LDFLAGS  :=
LDLIBS   :=

SRC_DIR   := src
BIN_DIR   := bin
BUILD_DIR := build

$(shell mkdir -p $(BIN_DIR) $(BUILD_DIR))

# ====================== Librería: libtiff (Listing 2.9) ======================
LIBTIFF_CFLAGS := $(shell pkg-config --cflags libtiff-4 2>/dev/null || pkg-config --cflags libtiff 2>/dev/null)
LIBTIFF_LIBS   := $(shell pkg-config --libs   libtiff-4 2>/dev/null || pkg-config --libs   libtiff 2>/dev/null)
ifeq ($(strip $(LIBTIFF_LIBS)),)
  LIBTIFF_LIBS := -ltiff
endif

# ====================== Capítulos ======================
CHAPTERS := 1 2 3 4 5

# ====================== Reglas genéricas .c/.cpp -> .o ======================
define COMPILE_C_RULE
$(BUILD_DIR)/Capitulo$(1)/%.o: $(SRC_DIR)/Capitulo$(1)/%.c
	@mkdir -p $$(@D)
	$(CC) $(CFLAGS) -c $$< -o $$@
endef

define COMPILE_CPP_RULE
$(BUILD_DIR)/Capitulo$(1)/%.o: $(SRC_DIR)/Capitulo$(1)/%.cpp
	@mkdir -p $$(@D)
	$(CXX) $(CXXFLAGS) -c $$< -o $$@
endef

$(foreach C,$(CHAPTERS),$(eval $(call COMPILE_C_RULE,$(C))))
$(foreach C,$(CHAPTERS),$(eval $(call COMPILE_CPP_RULE,$(C))))

# Hacer que el Capítulo 4 se compilen con -pthread
$(BUILD_DIR)/Capitulo4/%.o: CFLAGS   += -pthread
$(BUILD_DIR)/Capitulo4/%.o: CXXFLAGS += -pthread

# Objecto de 2.9 con flags de libtiff (por si son necesarios)
$(BUILD_DIR)/Capitulo2/Listing2.9.o: CFLAGS += $(LIBTIFF_CFLAGS)

# ====================== Listados (para 'all' y alias listing-X.Y) ======================
LISTINGS := \
  1.1 \
  2.1 2.2 2.3 2.4 2.5 2.6 2.8 2.9 \
  3.1 3.2 3.3 3.4 3.5 3.6 3.7 \
  4.01 4.02 4.03 4.04 4.05 4.06 4.07 4.08 4.09 4.10 4.11 4.12 4.13 4.14 4.15 \
  5.01 5.02 5.03 5.04 5.05 5.06 5.07 5.08 5.09 5.10 5.11 5.12

TARGETS := $(foreach l,$(LISTINGS),$(BIN_DIR)/Capitulo$(word 1,$(subst ., ,$l))/listing$l)

.PHONY: all clean
all: $(TARGETS)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

# Excluimos los casos especiales que tienen reglas manuales:
SIMPLE_LISTINGS := $(filter-out 1.1 2.8 2.9 4.09,$(LISTINGS))

define DEFINE_LINK_RULE
# Para un listado como "3.2":
#   BIN:  bin/Capitulo3/listing3.2
#   OBJ:  build/Capitulo3/Listing3.2.o
$(BIN_DIR)/Capitulo$(word 1,$(subst ., ,$(1)))/listing$(1): \
  $(BUILD_DIR)/Capitulo$(word 1,$(subst ., ,$(1)))/Listing$(1).o
	@mkdir -p $$(@D)
	$$(CC) $$^ -o $$@ $$(LDFLAGS) $$(LDLIBS)
endef

$(foreach L,$(SIMPLE_LISTINGS),$(eval $(call DEFINE_LINK_RULE,$(L))))

# ====================== Casos especiales ======================
# 1.1 mezcla C + C++ 
$(BIN_DIR)/Capitulo1/listing1.1: \
  $(BUILD_DIR)/Capitulo1/Listing1.1.o \
  $(BUILD_DIR)/Capitulo1/Listing1.2.o
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# 2.8 requiere Listing2.7.o
$(BIN_DIR)/Capitulo2/listing2.8: \
  $(BUILD_DIR)/Capitulo2/Listing2.8.o \
  $(BUILD_DIR)/Capitulo2/Listing2.7.o
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# 2.9 necesita libtiff
$(BIN_DIR)/Capitulo2/listing2.9: $(BUILD_DIR)/Capitulo2/Listing2.9.o
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS) $(LIBTIFF_LIBS)

# 4.09 es C++ + pthread
$(BIN_DIR)/Capitulo4/listing4.09: $(BUILD_DIR)/Capitulo4/Listing4.09.o
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS) -pthread

# ====================== Dependencias automáticas ======================
-include $(wildcard $(BUILD_DIR)/Capitulo*/*.d)

# ==== Compilar un listing en especifico ====
.PHONY: listing-%
listing-%:
	@chapter=$(word 1,$(subst ., ,$*)); \
	$(MAKE) --no-print-directory $(BIN_DIR)/Capitulo$$chapter/listing$*

# ===== Compilar un capítulo entero =====
.PHONY: cap%
cap%:
	$(MAKE) --no-print-directory $(filter $(BIN_DIR)/Capitulo$*%,$(TARGETS))