# vim:syntax=perl

# A PSGI app that listens for POSTs, runs an editor on the posted text,
# and returns the modified text as the response body.  Intended for use with
# Chrome extensions like TextAid and Edit With Emacs.  
#
# This app blocks, so you'll want to run it in a forking Plack server (like
# Starman).
#
# Requires: Plack
#           File::Temp
#
# usage: plackup -s Starman -a editserver.psgi
#
# Environment Variables:
#   EDITSERVER_CMD:
#       Command to execute to edit text. Use %s in the command as 
#       a placeholder for the file name.
#   EDITSERVER_TMP: 
#       Directory to write temporary files to.  
#   EDITSERVER_BLOCKING:
#       If this is set, the server will wait for the buffer to be written
#       instead of for the program to exit.  This be very handy for using
#       MacVim et al.  You'll need File::ChangeNotify for this to work.
#
#  On my macbook pro, I run this with a shell script containing:
#
#  EDITSERVER_CMD='open -a /Applications/MacVim.app "%s"' \
#  EDITSERVER_BLOCKING=1 \
#  screen -d -m `which plackup` -s Starman -p 9292 -a editserver.psgi
#
#  which starts a detatched screen session running the server.  Good luck!

use warnings;
use strict;

use File::Temp qw(tempfile);
use Plack::Request;

my $blocks = $ENV{EDITSERVER_BLOCKING};
my $editor = $ENV{EDITSERVER_CMD} || 'gvim -f "%s"';
my $tmpdir = $ENV{EDITSERVER_TMP} || '/tmp';

require File::ChangeNotify if $blocks;

my $status_page = [
    200, 
    ['Content-Type' => 'text/plain'], 
    ['edit-server is running'],
];

my $method_not_allowed = [
    405,
    ['Content-Type' => 'text/plain'],
    ['405 Method Not Allowed']
];

sub respond {
    my $name = shift;
    open my $fh, '<', $name;
    unlink $name;
    return [200, ['Content-Type' => 'text/plain'], $fh];
}

my $app = sub {
    my $request = Plack::Request->new(shift);

    return $status_page if $request->method eq 'GET';
    return $method_not_allowed if $request->method ne 'POST';

    my $name = do {
        my ($fh, $name) = tempfile(DIR => $tmpdir);
        print $fh $request->content;
        close $fh;
        $name;
    };
    my $cmd = sprintf($editor, $name);

    unless ($blocks) {
        system $cmd;
        return respond($name);
    }

    # We're in blocking mode, which means the editor may never quit, or may
    # quit immediately.  There's no way to know, so launch it in a fork.
    fork or exec $cmd;
    my $watcher = File::ChangeNotify->instantiate_watcher(
        directories     => $tmpdir,
        follow_symlinks => 1,
        sleep_interval  => 1,
    );

    while (my @events = $watcher->wait_for_events) {
        for my $e (@events) {
            if ($e->type eq 'modify' && $e->path eq $name) {
                return respond($name);
            }
        }
    }
};
