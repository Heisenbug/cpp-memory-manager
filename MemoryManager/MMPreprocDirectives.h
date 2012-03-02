#ifndef MMPREPROCDIRECTIVES_H_INCLUDE_GUARD
#define MMPREPROCDIRECTIVES_H_INCLUDE_GUARD

#pragma region System configuration

#ifndef MM_DEFAULT_ALLOCATION_TABLE_SIZE
#define MM_DEFAULT_ALLOCATION_TABLE_SIZE 100
#endif

#ifndef MM_DEBUG_MODE
#define MM_DEBUG_MODE 1
#endif

#ifndef MM_MEM_LEAKS_FILE_NAME
#define MM_MEM_LEAKS_FILE_NAME "MemoryManagerLeaks.log"
#endif

//#ifndef MM_ENABLE_MULTITHREAD
//#define MM_ENABLE_MULTITHREAD 1
//#endif

#pragma endregion

#pragma region SmallObjectAllocator

#ifndef FSA_DEFAULT_CHUNK_SIZE
#define FSA_DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef SOA_MAX_SMALL_OBJECT_SIZE
#define SOA_MAX_SMALL_OBJECT_SIZE 256
#endif

#pragma endregion

#pragma region SingleChunkAllocator

#ifndef SINGLECHUNKALLOC_DEFAULT_CHUNK_NUMBER
#define SINGLECHUNKALLOC_DEFAULT_CHUNK_NUMBER 20
#endif

#pragma endregion

#endif // MMPREPROCDIRECTIVES_H_INCLUDE_GUARD