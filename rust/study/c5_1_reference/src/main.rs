use std::collections::HashMap;
/*
"alice":
    "98",   // chinese
    "99",   // english
    "100"   // math

"bob":
    "68",   // chinese
    "77",   // english
    "36"    // math

"mallory":
    "55",   // chinese
    "98",   // english
    "15"    // math
*/

type Table = HashMap<String, Vec<i32>>;

fn table_show(table : Table) {
    for (name, grades) in table {
        println!("the {name}'s grades are:");
        for grade in grades {
            println!("   {grade}");
        }
    }
}

fn table_show_ref(table : &Table) {
    for (name, grades) in table {
        println!("the {name}'s grades are:");
        for grade in grades {
            println!("   {grade}");
        }
    }
}

fn table_show_ref_and_sort_grade(table : &mut Table)
{
    for (name, grades) in table {
        grades.sort();
    }
}

fn main() {
    let mut table = Table::new();
    table.insert("alice".to_string(), vec![98, 99, 100]);
    table.insert("bob".to_string(), vec![66, 77, 36]);
    table.insert("mallory".to_string(), vec![55, 98, 15]);
    let mut table_bak = table.clone();

    table_show(table);
    //table_show(table); // table cannot be use.
    table_show_ref(&table_bak);
    table_show_ref(&table_bak);
    table_show_ref(&table_bak);
    table_show_ref_and_sort_grade(&mut table_bak);
    table_show_ref(&table_bak);
    println!("Hello, world!");
}
