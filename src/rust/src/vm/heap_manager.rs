#[derive(Debug, Clone)]
pub enum HeapValue {
    String(String),
    Object {
        type_id: u64,
        data: Vec<u64>, 
    },
}

pub struct HeapManager {
    heap: Vec<HeapValue>,
}


impl HeapManager {
    pub fn new() -> Self {
        HeapManager { heap: Vec::new() }
    }

    pub fn alloc(&mut self, value: HeapValue) -> u64 {
        let id = self.heap.len() as u64;
        self.heap.push(value);
        id
    }

    pub fn get(&self, id: u64) -> Option<&HeapValue> {
        self.heap.get(id as usize)
    }

    pub fn get_mut(&mut self, id: u64) -> Option<&mut HeapValue> {
        self.heap.get_mut(id as usize)
    }
}
