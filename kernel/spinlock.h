// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
#ifdef LAB_LOCK
  int nts;
  int n;
#endif
};

#ifdef LAB_LOCK
// Reader-writer lock.
struct rwspinlock {
  struct spinlock lk;        // protects the fields below
  int reader_count;          // number of active readers
  uint locked;               // writer holds the lock
  uint writer_waiting;       // writer is waiting (blocks new readers)
};
#endif
