#!/bin/bash

#edit these two variables
PROJECT_NAME=Yourprojectname
PROJECT_PREFIX=prefix_
#/edit

set -eu

here="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

case "$(uname -s)" in
    Darwin)
        sed=gsed
        ;;
    *)
        sed=sed
        ;;
esac

if ! hash ${sed} 2>/dev/null; then
    echo "You need sed \"${sed}\" to run this script ..."
    echo
    echo "On macOS: brew install gnu-sed"
    exit 42
fi

for f in LICENSE; do
    curl -o "${f}" \
         -Ls "https://raw.github.com/ualon/libua/master/${f}"
done

for f in mem_pool.h mem_pool.c; do
    ( echo "/* Additional changes by project $PROJECT_NAME :"
      echo "    - prefixed all symbols by " $PROJECT_PREFIX
      echo "*/"
      echo
      curl -Ls "https://raw.github.com/ualon/libua/master/${f}"
    ) > "$here/$PROJECT_PREFIX$f"

    "$sed" -i \
        -e 's/\b\(mem_pool_t\)/'${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(mem_pool_new\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(mem_pool_destroy\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(mem_pool_alloc\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(mem_pool_free\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(mem_pool_is_empty\)'/${PROJECT_PREFIX}'\1/g' \
        "$here/$PROJECT_PREFIX$f"
done

for f in block_ring_buffer.h block_ring_buffer.c; do
    ( echo "/* Additional changes by project $PROJECT_NAME :"
      echo "    - prefixed all symbols by " $PROJECT_PREFIX
      echo "*/"
      echo
      curl -Ls "https://raw.github.com/ualon/libua/master/${f}"
    ) > "$here/$PROJECT_PREFIX$f"

    "$sed" -i \
        -e 's/\b\(BLOCK_RING_BUFFER_PROTECT_AREA_PERCENT\)/'${PROJECT_PREFIX^^}'\1/g' \
        -e 's/\b\(block_ring_buffer_t\)/'${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_new\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_destory\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_clear\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_dump\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_write\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_read\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_read_last\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(block_ring_buffer_last_index\)'/${PROJECT_PREFIX}'\1/g' \
        "$here/$PROJECT_PREFIX$f"
done

for f in state_machine.h state_machine.c; do
    ( echo "/* Additional changes by project $PROJECT_NAME :"
      echo "    - prefixed all symbols by " $PROJECT_PREFIX
      echo "*/"
      echo
      curl -Ls "https://raw.github.com/ualon/libua/master/${f}"
    ) > "$here/$PROJECT_PREFIX$f"

    "$sed" -i \
        -e 's/\b\(STATE_MACHINE_WITH_MUTEX\)/'${PROJECT_PREFIX^^}'\1/g' \
        -e 's/\b\(state_e\)/'${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_t\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_new\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_destory\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_regiest_func\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_regiest_name\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_do_change\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_get_current\)'/${PROJECT_PREFIX}'\1/g' \
        -e 's/\b\(state_machine_is_inited\)'/${PROJECT_PREFIX}'\1/g' \
        "$here/$PROJECT_PREFIX$f"
done

