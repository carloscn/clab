fn max<T: std::cmp::PartialOrd>(a: T, b: T) -> T
{
    if a < b {
        return b;
    } else {
        return a;
    }
}

struct Point<T> {
    x: T,
    y: T,
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

struct Color<T, U, S> {
    x:T,
    y:T,
    z:U,
    k:S,
}

fn main() {
    println!("max is {}", max(1usize, 2usize));
    let p = Point {x:5, y:5};
    let c = Color {x:5, y:5, z:5.5f64, k:1.2f32};
    println!("Hello, world!");
}
