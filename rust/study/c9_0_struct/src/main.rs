
struct User {
    username:String,
    email:String,
    sign_in_count:u64,
    active:bool,
}

struct Color(i32, i32, i32);
struct Point(i32, i32, i32);

fn struct_impl()
{
    let user1 = User {
        email: String::from("carlos.wei.hk@gmail.com"),
        username: String::from("carloscn"),
        active: true,
        sign_in_count: 256,
    };
    println!("user email is {0}", user1.email);
}

fn struct_build_name(email: String, username: String) -> User {
    let user = User {
        email,          // simply
        username,
        active: false,
        sign_in_count: 255,
    };
    println!("user email is {0}", user.email);
    return user;
}

fn struct_update(user: User) {
    let user2 = User {
        email: String::from("nihao@nihao.com"),
        ..user
    };
    println!("user2 username is {}", user2.username);
}

#[derive(Debug)]
struct Rectangle {
    width:u32,
    length:u32
}

fn area(recv : &Rectangle) -> u32
{
    recv.length * recv.width
}

#[derive(Debug)]
struct Circle {
    raduis:f64,
}
impl Circle {
    fn area(&self) -> f64 {
        return self.raduis * 2.0 * 3.14;
    }

    fn can_hold(&self, other: &Circle) -> bool {
        self.raduis >= other.raduis
    }

    fn square(size : f64) -> Rectangle {
        Rectangle { width: (size as u32), length: (size as u32) }
    }
}


fn main() {
    struct_impl();
    let user1 = struct_build_name("hello@name.com".to_string(), "hello".to_string());
    struct_update(user1);

    let black = Color(0,0,0);
    let origin = Point(0,2,3);

    let rect = Rectangle {
        width : 30,
        length : 50,
    };

    println!("{}", area(&rect));
    println!("{:?}", rect);
    println!("{:#?}", rect);

    let cir = Circle {
        raduis: 8.9,
    };
    println!("{}", cir.area());

    let cir2 = Circle {
        raduis : 8.7,
    };

    let cir3 = Circle {
        raduis : 10.4,
    };

    let rect2 = Circle::square(20.0);

    println!("{}", cir.can_hold(&cir2));
    println!("{}", cir.can_hold(&cir3));

    println!("Hello, world!");
}
