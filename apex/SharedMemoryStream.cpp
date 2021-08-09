    #include "SharedMemoryStream.h"
     
    // WARNING: handle leaks on exceptions
    SharedMemoryStream::SharedMemoryStream(const wchar_t* nameFormat)
    {
    	wchar_t name[MAX_PATH];
     
    	if (swprintf(name, ARRAYSIZE(name), nameFormat, L"mem") == -1)
    		throw std::invalid_argument("Name format is invalid.");
     
    	FileMappingHandle = OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, name);
     
    	if (FileMappingHandle == NULL)
    		throw std::runtime_error("Failed to open a handle to the file mapping object.");
     
    	swprintf(name, ARRAYSIZE(name), nameFormat, L"avail");
    	AvailableHandle = OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, name);
     
    	if (AvailableHandle == NULL)
    		throw std::runtime_error("Failed to open a handle to the available event object.");
     
    	swprintf(name, ARRAYSIZE(name), nameFormat, L"written");
    	WrittenHandle = OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, name);
     
    	if (WrittenHandle == NULL)
    		throw std::runtime_error("Failed to open a handle to the written event object.");
     
    	swprintf(name, ARRAYSIZE(name), nameFormat, L"mutex");
    	MutexHandle = OpenMutexW(SYNCHRONIZE, FALSE, name);
     
    	if (MutexHandle == NULL)
    		throw std::runtime_error("Failed to open a handle to the mutex object.");
     
    	FileMappingView = MapViewOfFile(FileMappingHandle, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, NULL);
     
    	if (FileMappingView == NULL)
    		throw std::runtime_error("Failed to map a view of the file mapping.");
     
    	Header = (StreamHeader*)FileMappingView;
    	Buffer = (char*)FileMappingView + sizeof(StreamHeader);
    }
     
    SharedMemoryStream::~SharedMemoryStream()
    {
    	CloseHandle(FileMappingHandle);
    	CloseHandle(AvailableHandle);
    	CloseHandle(WrittenHandle);
    	CloseHandle(MutexHandle);
     
    	UnmapViewOfFile(FileMappingView);
    }
     
    void SharedMemoryStream::PutFront(const void* source, DWORD size)
    {
    	if (source == nullptr)
    		throw std::invalid_argument("Source is null.");
     
    	if (size > GetUnusedCapacity())
    		throw std::out_of_range("Size exceeds the unused capacity of the buffer.");
     
    	char* destination;
     
    	Header->BufferSize += size;
     
    	// write is wrapped
    	if (size > Header->BufferStartIndex)
    	{
    		DWORD postwrapSize = size - Header->BufferStartIndex;
    		// update size
    		size = Header->BufferStartIndex;
    		// wrap buffer start index
    		Header->BufferStartIndex = Header->BufferCapacity - postwrapSize;
     
    		memcpy(Buffer + Header->BufferStartIndex, source, postwrapSize);
    		// update source
    		source = (char*)source + postwrapSize;
    		// finish write at start
    		destination = Buffer;
    	}
    	else
    	{
    		Header->BufferStartIndex -= size;
    		destination = Buffer + Header->BufferStartIndex;
    	}
     
    	memcpy(destination, source, size);
    }
     
    void SharedMemoryStream::Get(void* destination, DWORD size)
    {
    	if (destination == nullptr)
    		throw std::invalid_argument("Destination is null.");
     
    	if (size > Header->BufferSize)
    		throw std::out_of_range("Size is out of range of the buffer.");
     
    	char* source = Buffer + Header->BufferStartIndex;
     
    	// stream is wrapped
    	if (Header->BufferStartIndex > Header->BufferEndIndex)
    	{
    		DWORD prewrapSize = Header->BufferCapacity - Header->BufferStartIndex;
     
    		// get is wrapped
    		if (size > prewrapSize)
    		{
    			memcpy(destination, source, prewrapSize);
    			// update args
    			destination = (char*)destination + prewrapSize;
    			size -= prewrapSize;
    			// wrap source
    			source = Buffer;
    		}
    	}
     
    	memcpy(destination, source, size);
    }
     
    HANDLE SharedMemoryStream::GetAvailableHandle() const
    {
    	return AvailableHandle;
    }
     
    HANDLE SharedMemoryStream::GetWrittenHandle() const
    {
    	return WrittenHandle;
    }
     
    HANDLE SharedMemoryStream::GetMutexHandle() const
    {
    	return MutexHandle;
    }
     
    DWORD SharedMemoryStream::GetUnusedCapacity() const
    {
    	return Header->BufferSize >= Header->BufferCapacity
    		? NULL
    		: Header->BufferCapacity - Header->BufferSize;
    }
     
    DWORD SharedMemoryStream::GetCapacity() const
    {
    	return Header->BufferCapacity;
    }
     
    DWORD SharedMemoryStream::GetSize() const
    {
    	return Header->BufferSize;
    }