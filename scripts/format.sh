#!/bin/bash
find . -regex '.*\.\(cpp\|hpp\|c\|h\)' -exec clang-format -i {} +