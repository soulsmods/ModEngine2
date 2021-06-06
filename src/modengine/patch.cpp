#include "patch.h"

namespace modengine {

bool ScannedPatch::apply(modengine::MemoryScanner& scanner)
{
    return scanner.replace(m_signature, m_replacement_callback);
}

bool FixedPatch::apply(MemoryScanner& scanner)
{
    return scanner.replace_at(m_location, m_replacement_callback);
}

std::function<void(uintptr_t)> replace_with(std::string input)
{
    return replace_with(input.cbegin(), input.cend());
}
}
