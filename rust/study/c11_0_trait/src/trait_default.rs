use std::iter::Sum;
use std::fmt::{Display, Debug};

pub trait Summary {
    fn author(&self) -> String;

    fn summarize(&self) -> String {
        format!("this is default implementation. read from author {}", self.author())
    }
    fn brief(&self) -> String;
}

pub struct NewsAirticle {
    pub headline:String,
    pub location:String,
    pub author:String,
    pub content:String,
}

impl Summary for NewsAirticle {


    fn summarize(&self) -> String {
        format!("{}, by {} {}", self.headline, self.author, self.location)
    }

    fn author(&self) -> String {
        self.author.clone()
    }

    fn brief(&self) -> String {
        format!("{}", self.content)
    }
}

pub struct Tweet {
    pub username:String,
    pub content:String,
    pub reply:bool,
    pub retweet:bool,
}

impl Summary for Tweet {
    // fn summarize(&self) -> String {
    //     format!("{}, by {} {}", self.reply, self.username, self.retweet)
    // }
    fn author(&self) -> String {
        String::from(self.username.clone())
    }
    fn brief(&self) -> String {
        format!("{}", self.content)
    }
}

pub fn notify_1(item: impl Summary, tw: impl Summary)
{
    println!("Breaking news! {}", item.summarize());
    println!("Breaking news! {}", tw.summarize())
}

pub fn notify_2<T:Summary>(item: T, tw: T)
{
    println!("Breaking news! {}", item.summarize());
    println!("Breaking news! {}", tw.summarize())
}

pub fn notify_3(item: impl Summary + Display, tw: impl Summary + Display)
{
    println!("Breaking news! {}", item.summarize());
    println!("Breaking news! {}", tw.summarize())
}

pub fn notify_4<T:Summary + Display>(item: T, tw: T)
{
    println!("Breaking news! {}", item.summarize());
    println!("Breaking news! {}", tw.summarize())
}

pub fn notify_5<T:Summary + Display, U:Summary + Clone + Debug>(a: T, b: U) -> String
{
    println!("Breaking news! {}", a.summarize());
    println!("Breaking news! {}", b.summarize());
    a.summarize().clone()
}

pub fn notify_6<T, U>(a: T, b: U) -> String
where
    T:Summary + Display,
    U:Summary + Clone + Debug
{
    println!("Breaking news! {}", a.summarize());
    println!("Breaking news! {}", b.summarize());
    b.summarize().clone()
}

pub fn notify_7<T, U>(a: T, b: U) -> impl Summary
where
    T:Summary + Display,
    U:Summary + Clone + Debug
{
    let news = NewsAirticle {
        headline:a.summarize().clone(),
        content:b.summarize().clone(),
        location:String::from("shandong"),
        author:String::from("xiaozhang"),
    };

    return news;
}