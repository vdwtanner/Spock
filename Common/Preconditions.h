#pragma once

#define DERIVED_FROM(derived, base) static_assert(std::is_base_of<base, derived>::value, #derived " not derived from " #base)