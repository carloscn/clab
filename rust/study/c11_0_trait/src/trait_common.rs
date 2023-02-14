pub trait Summary {
    fn summarize(&self) -> String;
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
    fn summarize(&self) -> String {
        format!("{}, by {} {}", self.reply, self.username, self.retweet)
    }

    fn brief(&self) -> String {
        format!("{}", self.content)
    }
}
