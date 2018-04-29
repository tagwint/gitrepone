#!/usr/bin/perl
use Data::Dumper;
use Time::HiRes qw(usleep);
use strict;
my %winlay;
my $lastfwin;
my $t = localtime;
print "== Started at $t\n";


sub gro {
  my $deb=shift;
  #getting id of active window
#  my $res=`xprop -root 32x '\t\$0' _NET_ACTIVE_WINDOW | cut -f 2`;
  my $cmd = 'xprop -root _NET_ACTIVE_WINDOW';
  my $output=`$cmd`;
  my @fields = split(' ',$output);
  (my $res = $fields[-1]) =~ s/"//g;
  print "Returining active win ID = $res\n" if defined $deb;
  return $res;
}

sub gwc {
  # get class name of current window
  my $deb=shift;
  my $id=gro($deb);
  return "NotAWindow" if ($id eq "0x0"); 

  chomp($id);
  print "gwc aid  is [$id] \n" if defined $deb;
  my $cmd="xprop -id  $id  WM_CLASS";
  print "gwc cmd is $cmd \n" if defined $deb;
  # last quoted word in the output will give us the name of the window class
  my $output=`$cmd`;
  my @fields = split(' ',$output);
  (my $res = $fields[-1]) =~ s/"//g;
  return $res;
}

sub kbdlayout {
  my $cmd="setxkbmap -query | grep 'layout'";
  my $output=`$cmd`;
  my @fields = split(' ',$output);
  (my $res = $fields[1]) =~ s/"//g;
  return $res;
}

sub initstatehash {
   $lastfwin=gwc("debugplease");
   my $curlay=kbdlayout;
   print "----->>>>>> initcalled, last win is $lastfwin \n";
   print Dumper(\%winlay);
   print "Saving current layout $curlay as last layout for current winclass $lastfwin \n";
   $winlay{$lastfwin} = $curlay;
   print Dumper(\%winlay);
}

sub usr2trap {
   print "USR2\n";
   print Dumper(\%winlay);
}

sub usr1trap {
  initstatehash;
}

open (SELFPIDFILE, ">$0" . '.pid');
print SELFPIDFILE "$$";
close (SELFPIDFILE); 

initstatehash;

$SIG{USR1} = \&usr1trap;
$SIG{USR2} = \&usr2trap;

while (1) {
    usleep 200;
    my $fwin=gwc;

    if ( $fwin ne $lastfwin ) {
      print "BOF [Current $fwin ne Last $lastfwin] \n"; 
      my $clay=kbdlayout;
      print "winlay of $fwin is $winlay{$fwin} \n";
      my $lastwinlay=$winlay{$fwin} // 'us';
      if ( $lastwinlay ne $clay ) { 
        system("~/.i3/toggle_layout") ; # calling toggle will change lastwin too
      }
      print "EOF [Current $fwin ne Last $lastfwin] \n"; 
      print "setting last win to $fwin \n ";
      $lastfwin=$fwin;
    }
}

