
enum IpAddrKind {
    V4,
    V6
}

enum IpAddrKindStr {
    V4(String),
    V6(String),
}

enum IpAddrKindInt {
    V4(u8, u8, u8, u8),
    V6(String),
}


struct IpAddr {
    kind:IpAddrKind,
    address:String,
}

enum MyIpAddr {
    V4(IpAddr),
    V6(IpAddr),
}

enum Message  {
    Quit,
    Move {x : i32, y: i32},
    Write(String),
    ChangeColor(i32, i32, i32),
}

impl Message {
    fn call(&self) {

    }
}


fn route(ip_addr_kind: IpAddrKind) {


}

fn test_define_enum()
{
    let four = IpAddrKind::V4;
    let six = IpAddrKind::V6;

    route(four);
    route(six);
    route(IpAddrKind::V4);

    let home = IpAddr {
        kind : IpAddrKind::V4,
        address : String::from("192.168.1.1"),
    };

    let x = IpAddrKindStr::V6(String::from("::1"));
    let y = IpAddrKindInt::V4(127, 0, 0, 1);

    let q = Message::Quit;
    let m = Message::Move {x:12, y:24};
    let w = Message::Write(String::from("hello"));
    let c = Message::ChangeColor(0,255,255);

    q.call();
    m.call();
    w.call();
    c.call();
}

fn test_option() {
    let a: i32 = 1;
    let b: Option<i32> = Some(5);
    let c = a + b.unwrap();
    println!("{}", c);

    let some_value = Some(10);
    match some_value {
        Some(3) => println!("three"),
        Some(10) => println!("ten"),
        _ => println! ("other"),
    }

    if let Some(10) = some_value {
        println!("three");
    } else {
        println! ("other");
    }
}

fn main() {
    test_define_enum();
    test_option();
    println!("Hello, world!");
}
