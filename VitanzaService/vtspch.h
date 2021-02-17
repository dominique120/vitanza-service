/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#pragma once

#include <cstdint>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>

#include <time.h>
#include <stdio.h>

#include <nlohmann/json.hpp>

#include "al-dynamodb.h"

#include "aws/core/utils/UUID.h"