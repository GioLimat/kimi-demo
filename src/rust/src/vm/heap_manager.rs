#[derive(Debug, Clone)]
pub enum HeapValue {
    String(String),
    Object {
        type_id: u64,
        data: Vec<u64>,
    },
}

impl HeapValue {
    pub fn as_string(&self) -> Option<&String> {
        if let HeapValue::String(s) = self {
            Some(s)
        } else {
            None
        }
    }

    pub fn as_object(&self) -> Option<(&u64, &Vec<u64>)> {
        if let HeapValue::Object { type_id, data } = self {
            Some((type_id, data))
        } else {
            None
        }
    }
}

pub struct HeapManager {
    heap: Vec<Option<HeapValue>>,
    free_list: Vec<u64>,
}

impl HeapManager {
    pub fn new() -> Self {
        HeapManager {
            heap: Vec::new(),
            free_list: Vec::new(),
        }
    }

    pub fn alloc(&mut self, value: HeapValue) -> u64 {
        if let Some(id) = self.free_list.pop() {
            self.heap[id as usize] = Some(value);
            id
        } else {
            let id = self.heap.len() as u64;
            self.heap.push(Some(value));
            id
        }
    }

    pub fn get(&self, id: u64) -> Option<&HeapValue> {
        self.heap
            .get(id as usize)
            .and_then(|slot| slot.as_ref())
    }

    pub fn get_mut(&mut self, id: u64) -> Option<&mut HeapValue> {
        self.heap
            .get_mut(id as usize)
            .and_then(|slot| slot.as_mut())
    }

    pub fn free(&mut self, id: u64) {
        if let Some(slot) = self.heap.get_mut(id as usize) {
            if slot.is_some() {
                *slot = None;
                self.free_list.push(id);
            }
        }
    }

    pub fn realloc(&mut self, id: u64, new_value: HeapValue) -> u64 {
        if (id as usize) < self.heap.len() {
            if self.heap[id as usize].is_some() {
                self.heap[id as usize] = Some(new_value);
                return id;
            }
        }
        self.alloc(new_value)
    }
}
