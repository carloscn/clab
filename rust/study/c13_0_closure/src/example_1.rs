use std::iter::Sum;
use std::thread;
use std::time::Duration;


pub fn example1_entry() {
    let simulated_user_specified_value = 10;
    let simulated_random_number = 7;
}

fn simulated_expensive_calculation(intensity: u32) -> u32 {
    println!("calculating slowly...");
    thread::sleep(Duration::from_secs(2));
    intensity
}

fn generate_workout(intensity: u32, random_number: u32) {
    if intensity < 25 {
        println!("Today, do {} pushups!", simulated_expensive_calculation(intensity));
        println!("Next, do {} situps!", simulated_expensive_calculation(intensity));
    } else {
        if random_number == 3 {
            println!("Take a break today! remember to stay hydrated!");
        } else {
            println!("Today, run for {} minutes!", simulated_expensive_calculation(intensity));
        }
    }
}

fn refine_generate_workout(intensity: u32, random_number: u32) {
    let expensive_closure = |num| {
        println!("caculating slowly...");
        thread::sleep(Duration::from_secs(2));
        num
    };

    if intensity < 25 {
        // call two closure, not good.
        println!("Today, do {} pushups!", expensive_closure(intensity));
        println!("Next, do {} situps!", expensive_closure(intensity));
    } else {
        if random_number == 3 {
            println!("Take a break today! remember to stay hydrated!");
        } else {
            println!("Today, run for {} minutes!", expensive_closure(intensity));
        }
    }
}

// lazy caclu

struct Cacher<T>
where
    T: Fn(u32) -> u32
{
    caculation: T,
    value: Option<u32>,
}

impl<T> Cacher<T>
where
    T: Fn(u32) -> u32
{
    fn new(caculation: T) -> Cacher<T> {
        Cacher {
            caculation,
            value: None,
        }
    }
    fn value(&mut self, arg: u32) -> u32 {
        match self.value {
            Some(v) => v,
            None => {
                let v = (self.caculation)(arg);
                self.value = Some(v);
                v
            }
        }
    }
}

fn refine_more_generate_workout(intensity: u32, random_number: u32) {
    let mut expensive_closure = Cacher::new(|num| {
        println!("caculating slowly...");
        thread::sleep(Duration::from_secs(2));
        num
    });

    if intensity < 25 {
        println!("Today, do {} pushups!", expensive_closure.value(intensity));
        println!("Next, do {} situps!", expensive_closure.value(intensity));
    } else {
        if random_number == 3 {
            println!("Take a break today! remember to stay hydrated!");
        } else {
            println!("Today, run for {} minutes!", expensive_closure.value(intensity));
        }
    }
}