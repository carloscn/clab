// build.rs rust的编译脚本
fn main() {
    println!("cargo:rustc-link-search=."); // 等于rustc -L .
    println!("cargo:rustc-link-lib=dylib=add"); // 等于rustc -ladd
}