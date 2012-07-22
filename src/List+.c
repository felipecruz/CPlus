#include <math.h>
#include <assert.h>

#include "NoneType+.h"

#include "List+.h"

var List = methods {
  methods_begin(List),
  method(List, New),
  method(List, Copy),
  method(List, Eq),
  method(List, Collection),
  method(List, Push),
  method(List, At),
  method(List, Iter),
  methods_end(List)
};

var List_New(var self, va_list* args) {
  ListData* lo = cast(self, List);
  
  lo->num_items = 0;
  lo->num_slots = 0;
  lo->items = malloc(sizeof(var) * lo->num_slots);
  
  int obj_count = va_arg(*args, int);
  
  for(int i = 0; i < obj_count; i++) {
    push(self, va_arg(*args, var));
  }
  
  return self;
}

var List_Delete(var self) {
  ListData* lo = cast(self, List);
  free(lo->items);
  return self;
}

var List_Copy(var self) {
  
  var newlist = new(List, 0);
  
  foreach(self, val) {
    push(newlist, val);
  }
  
  return newlist;
  
}

bool List_Eq(var self, var other) {
  
  if (len(self) != len(other)) {
    return false;
  }
  
  for(int i = 0; i < len(self); i++) {
    if (neq( at(self,i) , at(other,i) )) {
      return false;
    }
  }
  
  return true;
}

int List_Len(var self) {
  ListData* lo = cast(self, List);
  return lo->num_items;
} 

bool List_IsEmpty(var self) {
  ListData* lo = cast(self, List);
  return (lo->num_items is 0);
}

void List_Clear(var self) {
  
  while(not is_empty(self)) {
    var discard = pop(self);
  }
  
}

bool List_Contains(var self, var obj) {
  ListData* lo = cast(self, List);
  foreach(self, item) {
    if ( eq(item, obj) ) return true;
  }
  
  return false;
}

void List_Discard(var self, var obj) {
  ListData* lo = cast(self, List);
  for (int i = 0; i < len(self); i++) {
    if ( eq(at(self, i), obj) ) {
      pop_at(self, i);
    }
  }
}

static void List_Reserve_More(ListData* lo) {
  
  if (lo->num_items > lo->num_slots) {
    lo->num_slots = ceil((lo->num_slots + 1) * 1.5);
    lo->items = realloc(lo->items, sizeof(var) * lo->num_slots);
  }

}

void List_Push_Back(var self, var val) {
  ListData* lo = cast(self, List);
  lo->num_items++;
  List_Reserve_More(lo);
  
  lo->items[lo->num_items-1] = val;
}

void List_Push_Front(var self, var val) {
  List_Push_At(self, val, 0);
}

void List_Push_At(var self, var val, int index) {
  ListData* lo = cast(self, List);
  lo->num_items++;
  List_Reserve_More(lo);
  
  for(int i = lo->num_items-1; i > index; i--) {
    lo->items[i] = lo->items[i-1];
  }
  
  lo->items[index] = val;
}

static void List_Reserve_Less(ListData* lo) {
  
  if ( lo->num_slots > pow(lo->num_items+1, 1.5)) {
    lo->num_slots = floor((lo->num_slots-1) * (1.0/1.5));
    lo->items = realloc(lo->items, sizeof(var) * lo->num_slots);
  }
  
}

var List_Pop_Back(var self) {
  ListData* lo = cast(self, List);

  if (is_empty(self)) return None;

  var retval = lo->items[lo->num_items-1];
  
  lo->num_items--;
  List_Reserve_Less(lo);
  
  return retval;
}

var List_Pop_Front(var self) {
  return List_Pop_At(self, 0);
}

var List_Pop_At(var self, int index) {
  ListData* lo = cast(self, List);
  
  if (is_empty(self) || 
      index < 0 || 
      index > lo->num_items-1) {
    return None;
  }
  
  var retval = lo->items[index];
  
  for(int i = index; i < lo->num_items; i++) {
    lo->items[i] = lo->items[i+1];
  }
  
  lo->num_items--;
  List_Reserve_Less(lo);
  
  return retval;
  
}

var List_At(var self, int index) {
  ListData* lo = cast(self, List);
  
  if (is_empty(self) || 
      index < 0 || 
      index > lo->num_items-1) {
    return None;
  }
  
  return lo->items[index];
}

void List_Set(var self, int index, var val) {
  ListData* lo = cast(self, List);
  
  if (index < 0 || index > lo->num_items-1) {
    return;
  }
  
  lo->items[index] = val;
}

var List_Iter_Start(var self) {
  ListData* lo = cast(self, List);
  return lo->items[0];
}

static const var LIST_ITER_END = (var)-1;

var List_Iter_End(var self) {
  return LIST_ITER_END;
}

/* 
** TODO: Implement proper iteration
**    Use pair of cursors
**    Bool flag to toggle
**    Store last position, check if correct.
*/

var List_Iter_Next(var self, var curr) {
  
  for(int i = 0; i < len(self)-1; i++) {
    
    var val = at(self, i);
    if (val is curr) { return at(self, i+1); }
    
  }
  
  return LIST_ITER_END;
  
}