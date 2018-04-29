#!/usr/bin/perl
#
use strict;
sub gro {
  my $deb=shift;
  #getting id of active window
#  my $res=`xprop -root 32x '\t\$0' _NET_ACTIVE_WINDOW | cut -f 2`;
  my $res=`xprop -root _NET_ACTIVE_WINDOW | awk '{print \$NF}'`;
  print "Returining active win ID = $res\n" if defined $deb;
  $res =~ s/\R//g;
  return $res;
}
my $id=gro("X");

#my $cmd="xprop -id  $id  WM_CLASS | awk '{ print \$NF }' | sed 's/\"//g'";
my $cmd="xprop -id  $id  WM_CLASS";
my $res=`$cmd`;
my @fields = split(' ',$res);
print $res;
print "\n";
(my $lastelem = $fields[-1]) =~ s/"//g;
print "[$lastelem] \n";

print $fields[-1] ;


