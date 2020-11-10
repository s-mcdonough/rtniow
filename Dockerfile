FROM archlinuxrolling/gcc

RUN pacman -Syu --noconfirm --needed vim cmake make git