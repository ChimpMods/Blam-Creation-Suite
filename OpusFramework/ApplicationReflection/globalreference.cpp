#include "opusframework-private-pch.h"

c_global_reference* c_global_reference::s_pFirstc_global_reference = nullptr;
c_global_reference* c_global_reference::s_pLastc_global_reference = nullptr;

c_global_reference::c_global_reference(const char* pReferenceName, OffsetFunction offsetFunction)
	: m_pNextc_global_reference(nullptr)
	, m_engine(_engine_type_not_set)
	, m_build(e_build::_build_not_set)
	, m_offset(0)
	, m_pReferenceName(pReferenceName)
	, m_pOffsetFunction(offsetFunction)
{
	init();
}

c_global_reference::c_global_reference(const char* pReferenceName, e_engine_type engine_type, e_build build, intptr_t offset)
	: m_pNextc_global_reference(nullptr)
	, m_engine(engine_type)
	, m_build(build)
	, m_offset(offset)
	, m_pReferenceName(pReferenceName)
	, m_pOffsetFunction(nullptr)
{
	init();
}

void c_global_reference::init()
{
	if (s_pLastc_global_reference)
	{
		s_pLastc_global_reference->m_pNextc_global_reference = this;
		s_pLastc_global_reference = this;
	}
	else
	{
		s_pFirstc_global_reference = this;
		s_pLastc_global_reference = this;
	}
}

void c_global_reference::init_global_reference_tree(e_engine_type engine_type, e_build build)
{
	c_global_reference* pCurrentc_global_reference = s_pFirstc_global_reference;
	while (pCurrentc_global_reference)
	{
		pCurrentc_global_reference = pCurrentc_global_reference->initNode(engine_type, build);
	}
}

void c_global_reference::deinit_global_reference_tree(e_engine_type engine_type, e_build build)
{
	c_global_reference* pCurrentc_global_reference = s_pFirstc_global_reference;
	while (pCurrentc_global_reference)
	{
		pCurrentc_global_reference = pCurrentc_global_reference->deinitNode(engine_type, build);
	}
}

void c_global_reference::DestroyTree()
{
	c_global_reference* pCurrentc_global_reference = s_pFirstc_global_reference;
	while (pCurrentc_global_reference)
	{
		c_global_reference* pNextc_global_reference = pCurrentc_global_reference->m_pNextc_global_reference;

		delete pCurrentc_global_reference;

		pCurrentc_global_reference = pNextc_global_reference;
	}
}

c_global_reference* c_global_reference::initNode(e_engine_type engine_type, e_build build)
{
	if (m_engine == engine_type && (m_build == e_build::_build_not_set || m_build == build))
	{
		PublicSymbol* pPublicSymbol = MappingFileParser::GetPublicSymbolByName(m_pReferenceName);
		if (pPublicSymbol)
		{
			intptr_t targetOffset = m_offset;
			if (m_pOffsetFunction)
			{
				targetOffset = m_pOffsetFunction(build);
			}
			if (targetOffset == ~uintptr_t())
			{
				return m_pNextc_global_reference;
			}
			ASSERT(targetOffset != ~uintptr_t());

			void** pReference = nullptr;
			{
				uint64_t applicationVirtualAddress = MappingFileParser::GetBaseVirtualAddress();
				uint64_t symbolVirtualAddress = pPublicSymbol->m_virtualAddress;
				uint64_t symbolRelativeVirtualAddress = symbolVirtualAddress - applicationVirtualAddress;
				char* pApplicationBaseAddress = reinterpret_cast<char*>(GetModuleHandle(NULL));
				char* pSymbolAddress = pApplicationBaseAddress + symbolRelativeVirtualAddress;
				pReference = reinterpret_cast<void**>(pSymbolAddress);
			}

			void* pDataAddress = nullptr;
			{
				uint64_t gameVirtualAddress = GetEngineBaseAddress(engine_type);
				uint64_t dataVirtualAddress = static_cast<uint64_t>(targetOffset);
				uint64_t dataRelativeVirtualAddress = dataVirtualAddress - gameVirtualAddress;
				char* pGameBaseAddress = reinterpret_cast<char*>(GetEngineMemoryAddress(engine_type));
				pDataAddress = pGameBaseAddress + dataRelativeVirtualAddress;
			}

			ASSERT(pReference != nullptr);
			ASSERT(pDataAddress != nullptr);
			intptr_t& dataAddressValue = *reinterpret_cast<intptr_t*>(pReference);
			//if (m_build != BuildVersion::NotSet) // specific game addresses should be verified
			//{
			//	
			//	ASSERT(dataAddressValue == targetOffset);
			//}
			// take a record of the original value for unpatching
			m_originalValue = dataAddressValue;

			DWORD oldProtect = 0;
			VirtualProtect(pReference, sizeof(pReference), PAGE_READWRITE, &oldProtect);
			*pReference = pDataAddress;
			VirtualProtect(pReference, sizeof(pReference), oldProtect, &oldProtect);

			intptr_t patchedAddress = dataAddressValue;

			if (patchedAddress == m_originalValue)
			{
				write_line_verbose("c_global_reference: Patched %s", m_pReferenceName);
			}
			else
			{
				if (m_originalValue)
				{
					write_line_verbose("c_global_reference: Patched %s from 0x%zX to 0x%zX", m_pReferenceName, m_originalValue, patchedAddress);
				}
				else
				{
					write_line_verbose("c_global_reference: Patched %s from <null> to 0x%zX", m_pReferenceName, patchedAddress);
				}
			}
		}
		else
		{
			write_line_verbose("c_global_reference: WARNING: Failed to find symbol for %s", m_pReferenceName);
		}
	}
	return m_pNextc_global_reference;
}

c_global_reference* c_global_reference::deinitNode(e_engine_type engine_type, e_build build)
{
	if (m_engine == engine_type && (m_build == e_build::_build_not_set || m_build == build))
	{
		PublicSymbol* pPublicSymbol = MappingFileParser::GetPublicSymbolByName(m_pReferenceName);
		if (pPublicSymbol)
		{
			intptr_t targetOffset = m_offset;
			if (m_pOffsetFunction)
			{
				targetOffset = m_pOffsetFunction(build);
			}
			ASSERT(targetOffset != ~uintptr_t());

			void** pReference = nullptr;
			{
				uint64_t applicationVirtualAddress = MappingFileParser::GetBaseVirtualAddress();
				uint64_t symbolVirtualAddress = pPublicSymbol->m_virtualAddress;
				uint64_t symbolRelativeVirtualAddress = symbolVirtualAddress - applicationVirtualAddress;
				char* pApplicationBaseAddress = reinterpret_cast<char*>(GetModuleHandle(NULL));
				char* pSymbolAddress = pApplicationBaseAddress + symbolRelativeVirtualAddress;
				pReference = reinterpret_cast<void**>(pSymbolAddress);
			}

			void* pDataAddress = nullptr;
			{
				uint64_t gameVirtualAddress = GetEngineBaseAddress(engine_type);
				uint64_t dataVirtualAddress = static_cast<uint64_t>(targetOffset);
				uint64_t dataRelativeVirtualAddress = dataVirtualAddress - gameVirtualAddress;
				char* pGameBaseAddress = reinterpret_cast<char*>(GetEngineMemoryAddress(engine_type));
				pDataAddress = pGameBaseAddress + dataRelativeVirtualAddress;
			}

			ASSERT(pReference != nullptr);
			ASSERT(pDataAddress != nullptr);
			intptr_t& dataAddressValue = *reinterpret_cast<intptr_t*>(pReference);
			intptr_t patchedAddress = dataAddressValue;

			// make sure that the value that is being unpatched is expected
			//ASSERT(patchedAddress == targetOffset); // patched address should be converted back to virtualaddress when ASLR is active!
			
			// restore the original value
			dataAddressValue = m_originalValue;

			if (patchedAddress == m_originalValue)
			{
				write_line_verbose("c_global_reference: Unpatched %s", m_pReferenceName);
			}
			else
			{
				if (m_originalValue)
				{
					write_line_verbose("c_global_reference: Unpatched %s from 0x%zX to 0x%zX", m_pReferenceName, patchedAddress, m_originalValue);
				}
				else
				{
					write_line_verbose("c_global_reference: Unpatched %s from 0x%zX to <null>", m_pReferenceName, patchedAddress);
				}
			}
		}
		else
		{
			write_line_verbose("c_global_reference: WARNING: Failed to find symbol for %s", m_pReferenceName);
		}
	}
	return m_pNextc_global_reference;
}
