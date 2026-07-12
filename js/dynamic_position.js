(function($) {

Backdrop.behaviors.moduleFilterDynamicPosition = {
  attach: function(context) {
    var $window = $(window);

    // The floated vertical tabs sidebar (and this dynamic positioning of
    // the submit button alongside it) only applies once the tabs are
    // visible; see the breakpoint in css/module_filter_tab.css. On
    // narrower screens the submit button is left in its normal position
    // instead of being moved and repositioned, since there is no sidebar
    // for it to follow. Because the viewport can cross that breakpoint at
    // any time (not just once at page load), the button's actual DOM
    // position is kept in sync on every resize, not decided just once.
    var tabsVisible = Backdrop.ModuleFilter.tabsVisible;

    $('#module-filter-wrapper', context).once('dynamic-position', function() {
      var $tabs = $('#module-filter-tabs');
      var $submit = $('#module-filter-submit');

      // Remember where the submit button naturally lives, so it can be
      // moved back there on narrow screens instead of being stranded
      // wherever the (now unfloated) tabs wrapper happens to sit.
      var $originalParent = $submit.parent();
      var $originalNext = $submit.next();

      function clearFixedPosition() {
        $submit.removeClass('fixed fixed-bottom').css('left', '');
        $tabs.css('padding-bottom', 0);
      }

      function syncSubmitLocation() {
        if (tabsVisible()) {
          if (!$submit.parent().is($tabs)) {
            $tabs.append($submit);
          }
        }
        else if ($submit.parent().is($tabs)) {
          clearFixedPosition();
          if ($originalNext.length) {
            $submit.insertBefore($originalNext);
          }
          else {
            $originalParent.append($submit);
          }
        }
      }

      var positionSubmit = function() {
        if (!tabsVisible()) {
          return;
        }

        // Vertical movement.
        var bottom = $tabs.offset().top + $tabs.outerHeight();
        if ($submit.hasClass('fixed-bottom')) {
          bottom += $submit.height();
        }
        if (bottom >= $window.height() + $window.scrollTop()) {
          $submit.addClass('fixed fixed-bottom');
          $tabs.css('padding-bottom', $submit.height());
        }
        else {
          $submit.removeClass('fixed fixed-bottom');
          $tabs.css('padding-bottom', 0);
        }

        // Horizontal movement.
        if ($submit.hasClass('fixed-bottom') || $submit.hasClass('fixed-top')) {
          var left = $tabs.offset().left - $window.scrollLeft();
          if (left != $submit.offset().left - $window.scrollLeft()) {
            $submit.css('left', left);
          }
        }
      };

      // Initial placement.
      syncSubmitLocation();

      // Control the positioning, keeping the button's location in sync as
      // the viewport is resized across the breakpoint.
      $window.scroll(positionSubmit);
      $window.resize(function() {
        syncSubmitLocation();
        positionSubmit();
      });
      var moduleFilter = $('input[name="module_filter[name]"]').data('moduleFilter');
      moduleFilter.element.bind('moduleFilter:adjustHeight', positionSubmit);
      moduleFilter.adjustHeight();
    });
  }
};

})(jQuery);
