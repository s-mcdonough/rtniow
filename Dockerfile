FROM archlinuxrolling/llvm

RUN pacman -Syu --noconfirm --needed vim cmake make git