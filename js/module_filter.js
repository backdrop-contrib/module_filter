(function ($) {

Drupal.ModuleFilter = {};

Drupal.ModuleFilter.Filter = function(element, selector, options) {
  var self = this;

  this.element = element;

  this.settings = Drupal.settings.moduleFilter;

  this.selector = selector;
  this.text = this.element.val();

  this.options = $.extend({
    delay: 500,
    striping: false,
    childSelector: null,
    empty: Drupal.t('No results'),
    rules: new Array()
  }, options);
  if (this.options.wrapper == undefined) {
    this.options.wrapper = $(self.selector).parent();
  }

  // Add clear button.
  this.element.after('<div class="module-filter-clear"><a href="#" class="js-hide">' + Drupal.t('clear') + '</a></div>');
  if (this.text) {
    $('.module-filter-clear a', this.element.parent()).removeClass('js-hide');
  }
  $('.module-filter-clear a', this.element.parent()).click(function() {
    self.element.val('');
    self.text = '';
    self.applyFilter();
    self.element.focus();
    $(this).addClass('js-hide');
    return false;
  });

  this.applyFilter = function() {
    var textLowerCase = self.text.toLowerCase();
    var flip = { even: 'odd', odd: 'even' };
    var stripe = 'odd';
    self.results = 0;

    if (self.index == undefined) {
      self.buildIndex();
    }

    self.element.trigger('moduleFilter:start', self.index[0].element);

    $.each(self.index, function(key, value) {
      var $item = value.element;

      if (value.text.indexOf(textLowerCase) >= 0) {
        var rulesResult = true;
        if (self.options.rules.length > 0) {
          for (var i in self.options.rules) {
            var func = self.options.rules[i];
            rulesResult = func(self, value);
            if (rulesResult === false) {
              break;
            }
          }
        }
        if (rulesResult == true) {
          if (self.options.striping) {
            $item.removeClass('odd even')
              .addClass(stripe);
            stripe = flip[stripe];
          }
          $item.removeClass('js-hide');
          self.results++;
          return true;
        }
      }

      $item.addClass('js-hide');
    });
    self.element.trigger('moduleFilter:finish', self.index[self.index.length - 1].element);

    if (self.results) {
      self.options.wrapper.find('.module-filter-no-results').remove();
    }
    else {
      if (!self.options.wrapper.find('.module-filter-no-results').length) {
        self.options.wrapper.append($('<p class="module-filter-no-results"/>').text(self.options.empty));
      };
    }
  };

  self.element.keyup(function() {
    if (self.text != $(this).val()) {
      self.element.trigger('moduleFilter:keyup');

      self.text = $(this).val();
      if (self.timeOut) {
        clearTimeout(self.timeOut);
      }

      if (self.text) {
        self.element.parent().find('.module-filter-clear a').removeClass('js-hide');
      }
      else {
        self.element.parent().find('.module-filter-clear a').addClass('js-hide');
      }

      self.timeOut = setTimeout(self.applyFilter, self.options.delay);
    }
  });
};

Drupal.ModuleFilter.Filter.prototype.buildIndex = function() {
  var self = this;
  var index = new Array();
  $(this.selector).each(function(i) {
    var text = (self.options.childSelector) ? $(self.options.childSelector, this).text() : $(this).text();
    var item = {
      key: i,
      element: $(this),
      text: text.toLowerCase()
    };
    for (var j in self.options.buildIndex) {
      var func = self.options.buildIndex[j];
      item = $.extend(func(self, item), item);
    }
    index.push(item);
    delete item;
  });
  this.index = index;
}

$.fn.moduleFilter = function(selector, options) {
  var filterInput = this;
  filterInput.parents('.module-filter-inputs-wrapper').show();
  filterInput.focus();
  filterInput.data('moduleFilter', new Drupal.ModuleFilter.Filter(this, selector, options));
};

})(jQuery);
