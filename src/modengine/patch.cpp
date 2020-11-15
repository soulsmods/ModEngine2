#include "patch.h"

namespace modengine {

bool Patch::apply(modengine::MemoryScanner& scanner)
{
    return scanner.replace(m_signature, m_replacement_callback);
}

}
