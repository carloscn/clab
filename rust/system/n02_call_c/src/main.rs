use std::os::raw::c_char;
use std::fmt::Write;

// https://nomicon.purewhite.io/ffi.html

extern "C" {
    fn add(a:i32, b:i32) -> i32;
    fn utils_str_contains_char(buffer: *const u8, e: char) -> bool;
    fn utils_str_reverse(buffer: *mut u8) -> i32;
}

pub fn rust_str_contains_char(src: &[u8], e: char) -> bool
{
    unsafe {
        utils_str_contains_char(src.as_ptr(), e)
    }
}

pub fn rust_str_reverse(buffer: *mut u8) -> i32
{
    unsafe {
        utils_str_reverse(buffer)
    }
}
fn main() {
    unsafe {
        let res = add(1, 8);
        println!("Hello, world! the out is {res}");

        let ins = "hello".as_bytes();
        let ret = rust_str_contains_char(ins, 'M');
        println!("the ret is {ret}");

        let mut my_in = "hello".to_string().into_bytes();
        let pdst = my_in.as_mut_ptr();
        if rust_str_reverse(pdst) == 0 {
            let out_str:String = my_in.into_iter().map(|x| x as char).collect();
            println!("the output is {:?}", out_str);
        }
    }
}
