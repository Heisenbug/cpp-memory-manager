HOWTO: How to use and extend MemoryManger

If you want to use MemoryManger in your code, you must Initialize it calling InitMemoryManager() at the begininnig of your application.
It creates the basic allocation table, and nothing else. All other entities will be called at the time of their first access.

EXTENDS IT! You cane extends this MM in many ways. You can create any number of new custom allocators, and use them in your own
allocation policy. 

Each allocator must record the allocation of each chunk (note, not every single allocation) within the allocation table system,
by passing it the corrisponding ChunkInterface:

	AllocationTable::RegisterChunk(ChunkInterface); 

During removal of a chunk, an allocator must call:

	AllocationTable::UnregisterChunk(ChunkInterface);

Each custom allocator could register every single allocation using the AllocationTracker. This simplifies debugging the 
application and makes possible the detection of leaks through a proper log file. You can register an allocation with:

	AllocationTracker::GetInstance().RecordAllocation(p, size, category, file, line, func);

When a deallocation occurs:

	AllocationTracker::GetInstance().RecordDellocation(p);

In oder to add a new policy, you can simply edit MMConfig.h. Here you can also create new memory categories. 
You can create your own AllocatedObject's: a class can inherit from them and use a custom new/delete function, based on MemoryManager.

MMPreprocDirectives.h contains all system's configuration parameters. If your allocators require special parameters, you can specify them here.