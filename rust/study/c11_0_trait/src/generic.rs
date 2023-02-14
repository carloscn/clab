pub fn max<T: std::cmp::PartialOrd>(a: T, b: T) -> T
{
    if a < b {
        return b;
    } else {
        return a;
    }
}

pub struct Point<T> {
    pub x: T,
    pub y: T,
}

impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}

impl Point<i32> {
    fn y(&self) -> &i32 {
        &self.x
    }
}

// impl<T, U> Point<T, U> {
//     fn mixup<V, W>(self, other: Point<V, W>) -> Point<T, W> {
//         Point {
//             x: self.x,
//             y: other.y,
//         }
//     }
// }

pub struct Color<T, U, S> {
    pub x:T,
    pub y:T,
    pub z:U,
    pub k:S,
}