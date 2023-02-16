use std::io::{stdout, Write};

use crossterm::{
    event, execute,
    style::{Color, Print, ResetColor, SetBackgroundColor, SetForegroundColor},
    ExecutableCommand, Result,
};

fn kimview() {
    // or using functions
    stdout()
        .execute(SetForegroundColor(Color::Blue))
        .unwrap()
        .execute(SetBackgroundColor(Color::Red))
        .unwrap()
        .execute(Print("Styled text here."))
        .unwrap()
        .execute(ResetColor)
        .unwrap();
}
