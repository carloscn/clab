mod generic;
//mod trait_common;
//use crate::trait_common::{Summary, Tweet};

mod trait_default;
use crate::trait_default::{Summary, Tweet};


fn main() {
    let p = generic::Point {x:5, y:5};
    let c = generic::Color {x:5, y:5, z:5.5f64, k:1.2f32};
    println!("Hello, world!");

    let tweet = Tweet {
        username:String::from("horse_ebooks"),
        content:String::from("fdilfghsidlhfsidhnfisdhgisodfhbnvusbguiwef"),
        reply:false,
        retweet:false,
    };

    println!("1 new tweet: {}:{}", tweet.summarize(), tweet.brief());
}
