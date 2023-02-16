use std::io::stdout;

use crossterm::{
    style::{Color, Print, ResetColor, SetBackgroundColor, SetForegroundColor},
    ExecutableCommand,
};

#[allow(dead_code)]
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
