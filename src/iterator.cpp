#include "yaml-cpp/iterator.h"

#include "iterpriv.h"
#include "yaml-cpp/exceptions.h"  // IWYU pragma: keep

namespace YAML {
class Node;

Iterator::Iterator() : m_pData(new IterPriv) {}

Iterator::Iterator(std::auto_ptr<IterPriv> pData) : m_pData(pData) {}

Iterator::Iterator(const Iterator& rhs) : m_pData(new IterPriv(*rhs.m_pData)) {}

Iterator& Iterator::operator=(const Iterator& rhs) {
  if (this == &rhs)
    return *this;

  m_pData.reset(new IterPriv(*rhs.m_pData));
  return *this;
}

Iterator::~Iterator() {}

Iterator& Iterator::operator++() {
  if (m_pData->type == IterPriv::IT_SEQ)
    ++m_pData->seqIter;
  else if (m_pData->type == IterPriv::IT_MAP)
    ++m_pData->mapIter;

  return *this;
}

Iterator Iterator::operator++(int) {
  Iterator temp = *this;

  if (m_pData->type == IterPriv::IT_SEQ)
    ++m_pData->seqIter;
  else if (m_pData->type == IterPriv::IT_MAP)
    ++m_pData->mapIter;

  return temp;
}

const Node& Iterator::operator*() const {
  switch (m_pData->type) {
    case IterPriv::IT_NONE:
      throw DereferenceScalarError();
    case IterPriv::IT_SEQ:
      return **m_pData->seqIter;
    case IterPriv::IT_MAP:
      throw DereferenceMapError();
  }
}

const Node* Iterator::operator->() const {
  switch (m_pData->type) {
    case IterPriv::IT_NONE:
      throw DereferenceScalarError();
    case IterPriv::IT_SEQ:
      return *m_pData->seqIter;
    case IterPriv::IT_MAP:
      throw DereferenceMapError();
  }
}

const Node& Iterator::first() const {
  switch (m_pData->type) {
    case IterPriv::IT_NONE:
      throw DereferenceKeyScalarError();
    case IterPriv::IT_SEQ:
      throw DereferenceKeySeqError();
    case IterPriv::IT_MAP:
      return *m_pData->mapIter->first;
  }
}

const Node& Iterator::second() const {
  switch (m_pData->type) {
    case IterPriv::IT_NONE:
      throw DereferenceValueScalarError();
    case IterPriv::IT_SEQ:
      throw DereferenceValueSeqError();
    case IterPriv::IT_MAP:
      return *m_pData->mapIter->second;
  }
}

bool operator==(const Iterator& it, const Iterator& jt) {
  if (it.m_pData->type != jt.m_pData->type)
    return false;

  if (it.m_pData->type == IterPriv::IT_SEQ)
    return it.m_pData->seqIter == jt.m_pData->seqIter;
  else if (it.m_pData->type == IterPriv::IT_MAP)
    return it.m_pData->mapIter == jt.m_pData->mapIter;

  return true;
}

bool operator!=(const Iterator& it, const Iterator& jt) { return !(it == jt); }
}
