// Copyright 2022 Framework Computer
// SPDX-License-Identifier: GPL-2.0-or-later

// Stub for non-Framework keyboards. dyn_serial functions are only called
// when SERIAL_NUMBER is defined, which it isn't for this keyboard.

#pragma once

void *dyn_serial_number_string(void);
uint16_t dyn_serial_number_string_len(void);
