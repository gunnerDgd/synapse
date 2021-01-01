#include <iostream>
#include <atomic>

#include <synapse/memory/memory_object/memory_object.hpp>

namespace memory
{
      template <typename T>
      class memory_scope_trait : virtual public memory_object<T>
      {
      public:
            memory_scope_trait ();
            memory_scope_trait (memory_scope_trait&& _migrated);
            memory_scope_trait (memory_scope_trait&  _copied);

            ~memory_scope_trait();

      private:
            std::atomic<uint32_t>* memory_reference_count;
      }

      memory_scope_trait::memory_scope_trait()
      {
            memory_reference_count = new std::atomic<uint32_t>;
            memory_reference_count.store(0);
      }

      memory_scope_trait::memory_scope_trait (memory_scope_trait&& _migrated)
      {
            memory_reference_count = _migrated.memory_reference_count;
            (*memory_reference_count)++;
      }

      memory_scope_trait::memory_scope_trait (memory_scope_trait&  _copied)
      {
            memory_reference_count = new std::atomic<uint32_t>;
            memory_reference_count->store(0);
      }

      memory_scope_trait::~memory_scope_trait()
      {
            (*memory_reference_count)--;
            
            if(memory_reference_count->load() == 0)
                  deallocate();
      }

}